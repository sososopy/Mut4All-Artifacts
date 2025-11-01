use crate::mutator::Mutator;
use syn::{parse_quote, File, Item, GenericParam, Stmt, Expr, PathArguments, GenericArgument, LocalInit};

pub struct Add_Inherent_Assoc_Type_Const_Generics_212;

impl Mutator for Add_Inherent_Assoc_Type_Const_Generics_212 {
    fn name(&self) -> &str {
        "Add_Inherent_Assoc_Type_Const_Generics_212"
    }
    fn mutate(&self, file: &mut File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generic_param) = item_struct.generics.params.first() {
                    if let GenericParam::Const(_) = generic_param {
                        let struct_ident = &item_struct.ident;
                        let impl_block: Item = parse_quote! {
                            impl #struct_ident {
                                type ComplexType<const N: usize> = #struct_ident<N>;
                            }
                        };
                        new_items.push(impl_block);
                    }
                }
            }
        }
        file.items.extend(new_items);

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Some(init) = &local.init {
                                if let Expr::Call(expr_call) = init.expr.as_ref() {
                                    if let Expr::Path(expr_path) = expr_call.func.as_ref() {
                                        if expr_path.path.segments.len() == 3 {
                                            if let Some(last_segment) = expr_path.path.segments.last() {
                                                if last_segment.ident == "create" {
                                                    let new_expr: Expr = parse_quote!({20 / 2});
                                                    if let PathArguments::AngleBracketed(angle_bracketed) = &expr_path.path.segments[1].arguments {
                                                        if let Some(GenericArgument::Const(_)) = angle_bracketed.args.first() {
                                                            let mut new_args = angle_bracketed.args.clone();
                                                            new_args = parse_quote!(<#new_expr>);
                                                            let mut segments = expr_path.path.segments.clone();
                                                            segments[1].arguments = PathArguments::AngleBracketed(parse_quote!(<#new_args>));
                                                            let mut new_path = expr_path.path.clone();
                                                            new_path.segments = segments;
                                                            *expr_path = Expr::Path(parse_quote!(#new_path));
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
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structures with const generics and introduces inherent associated types with const generics in their implementation. It then modifies function calls to use complex const expressions, testing the compiler's ability to handle such constructs."
    }
}