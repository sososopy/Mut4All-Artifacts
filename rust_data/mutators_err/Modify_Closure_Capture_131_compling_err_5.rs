use crate::mutator::Mutator;

pub struct Modify_Closure_Capture_131;

impl Mutator for Modify_Closure_Capture_131 {
    fn name(&self) -> &str {
        "Modify_Closure_Capture_131"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    let new_var: Stmt = parse_quote! {
                        let buffer: &[u8] = b"bar";
                    };
                    item_fn.block.stmts.insert(0, new_var);

                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Closure(closure)) = stmt {
                            if let Expr::Block(block) = &mut *closure.body {
                                if let Some(first_stmt) = block.block.stmts.first_mut() {
                                    if let Stmt::Local(local) = first_stmt {
                                        if let Some(init) = &mut local.init {
                                            init.expr = Box::new(parse_quote!(buffer));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify closures within const functions and introduce a variable outside the closure, then modify the closure to capture this variable by using it inside."
    }
}