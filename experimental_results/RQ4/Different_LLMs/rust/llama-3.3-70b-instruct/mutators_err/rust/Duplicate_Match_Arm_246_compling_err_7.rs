use rand::Rng;
use rand::thread_rng;
use syn::{File, Item, Stmt, Expr, Arm, Block};

struct Duplicate_Match_Arm_246;

impl Duplicate_Match_Arm_246 {
    pub fn new() -> Self {
        Duplicate_Match_Arm_246
    }
}

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Duplicate_Match_Arm_246 {
    fn name(&self) -> &str {
        "Duplicate_Match_Arm_246"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Match(match_expr) = expr {
                            let arms = &match_expr.arms;
                            if arms.len() > 1 {
                                let mut rng = thread_rng();
                                let index_to_duplicate = rng.gen_range(0..arms.len());
                                let arm_to_duplicate = arms[index_to_duplicate].clone();
                                match_expr.arms.push(arm_to_duplicate);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The Duplicate_Match_Arm_246 mutation operator targets match statements in the Rust code and duplicates one of its arms. This transformation can trigger bugs related to pattern matching and compilation, such as duplicate match arms, and tests the compiler's handling of match statements with multiple arms."
    }
}