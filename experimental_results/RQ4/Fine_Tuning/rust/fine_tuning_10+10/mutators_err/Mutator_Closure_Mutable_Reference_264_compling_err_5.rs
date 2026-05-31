use syn::{parse_quote, Expr, visit_mut::VisitMut, ExprClosure, FnArg, PatType, Type, Item, ItemFn, Stmt};

pub struct Mutator_Closure_Mutable_Reference_264;

impl Mutator for Mutator_Closure_Mutable_Reference_264 {
    fn name(&self) -> &str {
        "Mutator_Closure_Mutable_Reference_264"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureVisitor<'a> {
            closure_found: bool,
            closure_expr: Option<&'a mut Expr>,
        }

        impl<'a> VisitMut for ClosureVisitor<'a> {
            fn visit_expr_closure_mut(&mut self, i: &mut ExprClosure) {
                if self.closure_found {
                    return;
                }
                
                if i.inputs.len() > 0 {
                    let mut has_mutable_borrow = false;
                    for input in &i.inputs {
                        if let FnArg::Typed(PatType { ty, .. }) = input {
                            if let Type::Reference(tr) = &**ty {
                                if tr.mutability.is_some() {
                                    has_mutable_borrow = true;
                                    break;
                                }
                            }
                        }
                    }

                    if has_mutable_borrow {
                        self.closure_found = true;
                        self.closure_expr = Some(&mut i.body);
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(ItemFn { block, .. }) = item {
                let mut visitor = ClosureVisitor {
                    closure_found: false,
                    closure_expr: None,
                };
                visitor.visit_block_mut(block);

                if let Some(closure_expr) = visitor.closure_expr {
                    if let Expr::Block(expr_block) = closure_expr {
                        let stmts = &mut expr_block.block.stmts;
                        if stmts.len() > 0 {
                            let first_stmt = &stmts[0];
                            if let Stmt::Expr(Expr::MethodCall(method_call)) = first_stmt {
                                let receiver = &method_call.receiver;
                                let new_stmt: Stmt = parse_quote! {
                                    let immutable_ref = &receiver.some_field;
                                };
                                stmts.insert(0, new_stmt);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation targets closures within functions that capture mutable references. It introduces a new immutable borrow before a mutable borrow within the closure, testing the borrow checker's handling of conflicting borrows in the same scope."
    }
}