use syn::parse_quote;
use syn::{visit_mut::VisitMut, Expr, ExprLit, Lit, visit_mut};

trait Mutator {
    fn name(&self) -> &str;
    fn chain_of_thought(&self) -> &str;
    fn mutate(&self, file_ast: &mut syn::File);
}

struct ReplaceConstValue;

impl Mutator for ReplaceConstValue {
    fn name(&self) -> &str {
        "ReplaceConstValue"
    }

    fn chain_of_thought(&self) -> &str {
        "Replace constant values in the AST"
    }

    fn mutate(&self, file_ast: &mut syn::File) {
        struct ReplaceConstValueMutator;

        impl VisitMut for ReplaceConstValueMutator {
            fn visit_expr_mut(&mut self, node: &mut Expr) {
                if let Expr::Lit(ExprLit {
                    lit:
                        Lit::Int(int_lit),
                    ..
                }) = node
                {
                    int_lit.base10_parse::<i32>().unwrap();
                    *node = syn::parse_quote!(42);
                }
                visit_mut::visit_expr_mut(self, node)
            }
        }

        ReplaceConstValueMutator.visit_file_mut(file_ast);
    }
}

fn main() {
    let mut file_ast = parse_quote! {
        const X: i32 = 10;
        fn main() {
            let x = X;
        }
    };
    let mutator = ReplaceConstValue;
    mutator.mutate(&mut file_ast);
}