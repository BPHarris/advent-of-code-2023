pub fn gcd(mut a: u64, mut b: u64) -> u64 {
    while b != 0 {
        let temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

pub fn lcm(mut nums: Vec<u64>) -> u64 {
    let mut lcm = nums.remove(0);
    for &num in &nums {
        lcm = lcm * num / gcd(lcm, num);
    }
    return lcm;
}
