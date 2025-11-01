use crate::mutator::Mutator;
use syn::{parse_quote, Item, GenericParam, Type, PathArguments, GenericArgument, Expr, TypePath, ImplItem, Stmt, ExprBlock, Token};

pub struct Modify_Const_Generic_Expressions_163;

impl Mutator for Modify_Const_Generic_Expressions_163 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Expressions_163"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut impl_items = Vec::new();
        let items_clone = file.items.clone();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = item_struct.generics.params.first() {
                    if let GenericParam::Const(_) = generics {
                        for item in &items_clone {
                            if let Item::Impl(item_impl) = item {
                                if item_impl.generics.params.iter().any(|param| matches!(param, GenericParam::Const(_))) {
                                    for item in &item_impl.items {
                                        if let ImplItem::Type(impl_type) = item {
                                            if let Type::Path(type_path) = &impl_type.ty {
                                                if let Some(segment) = type_path.path.segments.last() {
                                                    if segment.ident == "Store" {
                                                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                                            if let Some(GenericArgument::Const(Expr::Block(expr_block))) = args.args.first() {
                                                                let mut expr = expr_block.block.stmts.clone();
                                                                expr.push(Stmt::Expr(parse_quote!(undefined_var + 1), None));
                                                                impl_items.push((impl_type.clone(), expr));
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
        for (mut impl_type, expr) in impl_items {
            impl_type.ty = Type::Path(TypePath {
                qself: None,
                path: parse_quote!(Store<{ #(#expr)* }>),
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs with const generics, modifying expressions in const parameters. It introduces arithmetic operations with undefined or mismatched types, increasing the likelihood of triggering compiler issues."
    }
}