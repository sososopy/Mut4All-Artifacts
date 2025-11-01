use crate::mutator::Mutator;
use syn::{parse_quote, Expr, Item, Stmt, LocalInit};

pub struct Modify_Static_Init_With_Drop_464;

impl Mutator for Modify_Static_Init_With_Drop_464 {
    fn name(&self) -> &str {
        "Modify_Static_Init_With_Drop_464"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(static_item) = item {
                if let Expr::Block(expr_block) = &mut *static_item.expr {
                    let mut new_stmts = vec![];
                    for stmt in &expr_block.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(LocalInit { expr: init, .. }) = &local.init {
                                if let Expr::Array(_) | Expr::Call(_) = &**init {
                                    let stmt1: Stmt = parse_quote! {
                                        let mut test_vec: Vec<String> = Vec::new();
                                    };
                                    let stmt2: Stmt = parse_quote! {
                                        test_vec.push(String::from("Example"));
                                    };
                                    new_stmts.push(stmt1);
                                    new_stmts.push(stmt2);
                                }
                            }
                        }
                    }
                    expr_block.block.stmts.extend(new_stmts);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static initialization blocks that include data structures with Drop traits. It introduces a new vector with a Drop trait element and directly assigns an element to it without prior initialization. This mutation tests the compiler's handling of uninitialized memory and destructors during static initialization, potentially leading to ICE scenarios."
    }
}