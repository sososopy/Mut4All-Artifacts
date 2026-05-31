use syn::{parse_quote, Item, ItemStruct, GenericParam, Type, TypePath, PathArguments, GenericArgument, Expr, Stmt};

pub struct Alter_Macro_With_Const_Generics_75;

impl Mutator for Alter_Macro_With_Const_Generics_75 {
    fn name(&self) -> &str {
        "Alter_Macro_With_Const_Generics_75"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, fields, .. }) = item {
                if let Some(GenericParam::Const(const_param)) = generics.params.first() {
                    if let Some(field) = fields.iter_mut().next() {
                        if let Type::Path(TypePath { path, .. }) = &field.ty {
                            if let Some(segment) = path.segments.last() {
                                if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                    if let Some(GenericArgument::Const(Expr::Block(expr_block))) = args.args.first() {
                                        if let Some(Stmt::Expr(Expr::Macro(expr_macro), _)) = expr_block.block.stmts.first_mut() {
                                            let new_macro = parse_quote! {
                                                macro_rules! outer_wrap {
                                                    ($x:expr) => {
                                                        #expr_macro
                                                    };
                                                }
                                            };
                                            let new_expr: Expr = parse_quote! {
                                                outer_wrap!(#const_param.ident)
                                            };
                                            let mut new_expr_macro = expr_macro.clone();
                                            new_expr_macro.mac.tokens = new_expr.into_token_stream();
                                            expr_block.block.stmts[0] = Stmt::Expr(Expr::Macro(new_expr_macro), None);
                                            new_items.push(Item::Macro(new_macro));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        file.items.splice(0..0, new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions using const generics with macros. It introduces an additional macro layer to wrap existing macro invocations within const generic parameters. This stresses the Rust compiler's macro expansion handling in const contexts, potentially leading to internal compiler errors or unexpected behavior."
    }
}