use crate::mutator::Mutator;
use syn::{parse_quote, Item, PathArguments, GenericArgument, Expr, PathSegment};

pub struct Alter_Array_Size_In_Generic_Constants_169;

impl Mutator for Alter_Array_Size_In_Generic_Constants_169 {
    fn name(&self) -> &str {
        "Alter_Array_Size_In_Generic_Constants_169"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &mut item_impl.trait_ {
                    let segments: &mut Vec<PathSegment> = &mut path.segments;
                    for segment in segments {
                        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Const(expr) = arg {
                                    if let Expr::Path(expr_path) = expr {
                                        if let Some(ident) = expr_path.path.get_ident() {
                                            let doubled_expr: Expr = parse_quote!(#ident * 2);
                                            let incremented_expr: Expr = parse_quote!(#ident + 1);

                                            // Apply mutation by doubling the size
                                            *arg = GenericArgument::Const(doubled_expr.clone());
                                            
                                            // Alternatively, apply mutation by incrementing the size
                                            // *arg = GenericArgument::Const(incremented_expr.clone());
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
        "The mutation operator targets array sizes in generic constants within function signatures or implementations. It identifies arrays with expressions like `[T; N]` and modifies the size by either doubling it or adding a fixed increment. This mutation is designed to test the robustness of code handling array sizes, potentially uncovering assumptions or errors related to array bounds."
    }
}