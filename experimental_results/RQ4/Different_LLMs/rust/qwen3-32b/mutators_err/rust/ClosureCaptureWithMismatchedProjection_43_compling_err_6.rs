use syn::parse_quote;
use crate::mutator::Mutator;
use syn::visit_mut::VisitMut;
use syn::Stmt;
use syn::Expr;

pub struct ClosureCaptureWithMismatchedProjection_43;

impl Mutator for ClosureCaptureWithMismatchedProjection_43 {
    fn name(&self) -> &str {
        "ClosureCaptureWithMismatchedProjection_43"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = ClosureMutatorVisitor {};
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a closure that captures a mutable reference to a tuple struct, then destructures it with a pattern that introduces a new variable and an unused wildcard, creating a type mismatch. This stresses the compiler's borrow checker and type system by combining closure capture, field projection, and invalid pattern matching."
    }
}

struct ClosureMutatorVisitor;

impl VisitMut for ClosureMutatorVisitor {
    fn visit_stmt_mut(&mut self, stmt: &mut Stmt) {
        if let Stmt::Expr(expr, _) = stmt {
            if let Expr::Assign(assign_expr) = &*expr {
                if let Expr::Field(field_expr) = &*assign_expr.left {
                    if let Expr::Path(path_expr) = &*field_expr.base {
                        if let Some(ident) = path_expr.path.get_ident() {
                            let rhs = assign_expr.right.clone();
                            let lhs_pattern: syn::Pat = parse_quote! { V((x, _y)) };
                            let rhs_expr: syn::Expr = parse_quote! { #ident };
                            let assign_pattern = parse_quote! { #lhs_pattern = #rhs_expr; };
                            let original_assign = parse_quote! { #ident.0 = #rhs; };
                            let closure_body = parse_quote! {
                                {
                                    #assign_pattern
                                    #original_assign
                                }
                            };
                            let closure = parse_quote! {
                                || #closure_body
                            };
                            *expr = Expr::Closure(closure);
                            return;
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_stmt_mut(self, stmt);
    }
}