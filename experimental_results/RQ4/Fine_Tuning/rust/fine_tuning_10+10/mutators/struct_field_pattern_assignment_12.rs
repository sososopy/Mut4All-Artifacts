use syn::parse_quote;
use syn::Expr;
use syn::ExprClosure;
use syn::visit_mut::{self, VisitMut};
use crate::mutator::Mutator;

pub struct Struct_Field_Pattern_Assignment_12;

impl Mutator for Struct_Field_Pattern_Assignment_12 {
    fn name(&self) -> &str {
        "Struct_Field_Pattern_Assignment_12"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureVisitor<'a> {
            mutations: &'a mut Vec<Expr>,
        }

        impl<'a> VisitMut for ClosureVisitor<'a> {
            fn visit_expr_closure_mut(&mut self, i: &mut ExprClosure) {
                if let Expr::Assign(assign_expr) = &mut *i.body {
                    if let Expr::Path(expr_path) = &*assign_expr.left {
                        if let Some(segment) = expr_path.path.segments.last() {
                            if segment.ident.to_string().starts_with('S') {
                                let new_pattern: Expr = parse_quote! { S(x) };
                                self.mutations.push(new_pattern.clone());
                                assign_expr.left = Box::new(new_pattern);
                            }
                        }
                    }
                }
                visit_mut::visit_expr_closure_mut(self, i);
            }
        }

        let mut mutations = Vec::new();
        let mut visitor = ClosureVisitor { mutations: &mut mutations };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator focuses on struct field pattern assignments within closures. It identifies assignments where a struct pattern is used and modifies the pattern to be incorrect by underspecifying it, such as leaving fields unmatched. This can lead to unexpected compiler behavior as it attempts to resolve the mismatched pattern."
    }
}