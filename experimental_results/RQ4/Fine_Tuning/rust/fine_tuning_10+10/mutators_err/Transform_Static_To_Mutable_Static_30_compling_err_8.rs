use crate::mutator::Mutator;
use syn::parse_quote;
use syn::Item;
use syn::StaticMutability;
use syn::token::Mut;
use syn::Ident;
use syn::Expr;
use syn::visit::Visit;
use syn::ExprPath;

pub struct Transform_Static_To_Mutable_Static_30;

impl Mutator for Transform_Static_To_Mutable_Static_30 {
    fn name(&self) -> &str {
        "Transform_Static_To_Mutable_Static_30"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut usages = Vec::new();

        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if matches!(item_static.mutability, StaticMutability::None) {
                    item_static.mutability = StaticMutability::Mut(Mut::default());

                    let ident = &item_static.ident;

                    struct FindUsages<'a> {
                        ident: &'a Ident,
                        usages: &'a mut Vec<Expr>,
                    }

                    impl<'a> Visit<'_> for FindUsages<'a> {
                        fn visit_expr(&mut self, expr: &Expr) {
                            if let Expr::Path(ExprPath { path, .. }) = expr {
                                if path.is_ident(self.ident) {
                                    self.usages.push(expr.clone());
                                }
                            }
                            syn::visit::visit_expr(self, expr);
                        }
                    }

                    let mut finder = FindUsages { ident, usages: &mut usages };
                    finder.visit_file(&*file);
                }
            }
        }

        for usage in &mut usages {
            let unsafe_block: Expr = parse_quote! {
                unsafe { #usage }
            };
            *usage = unsafe_block;
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static variables that are initialized as immutable. It transforms them into mutable static variables by changing the static keyword to static mut. This requires ensuring semantic correctness by wrapping usages of the static variable in an unsafe block, allowing for mutable modifications. This mutation can reveal issues in constant evaluation and unsafe code handling."
    }
}