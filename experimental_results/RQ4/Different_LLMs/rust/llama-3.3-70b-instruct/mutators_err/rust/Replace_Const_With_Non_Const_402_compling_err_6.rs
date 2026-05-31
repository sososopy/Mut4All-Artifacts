use syn::{parse_quote, Item, Pat, PatIdent, ImplItem, token, ItemLet};
use crate::mutator::Mutator;

struct Replace_Const_With_Non_Const_402;

impl Mutator for Replace_Const_With_Non_Const_402 {
    fn name(&self) -> &str {
        "Replace_Const_With_Non_Const_402"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                let const_token = item_const.const_token;
                let ident = item_const.ident;
                let ty = item_const.ty;
                let expr = item_const.expr;
                let vis = item_const.vis;
                let attrs = item_const.attrs;

                *item = Item::Let(syn::ItemLet {
                    attrs,
                    let_token: token::Let {
                        spans: Default::default(),
                    },
                    pat: Pat::Ident(PatIdent {
                        attrs: vec![],
                        by_ref: None,
                        mutability: None,
                        ident,
                        subpat: None,
                    }),
                    ty: Some(ty),
                    init: Some(expr),
                    semi_token: token::Semi::default(),
                });
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Const(item_const) = impl_item {
                        let const_token = item_const.const_token;
                        let ident = item_const.ident;
                        let ty = item_const.ty;
                        let expr = item_const.expr;
                        let vis = item_const.vis;
                        let attrs = item_const.attrs;

                        *impl_item = ImplItem::Const(syn::ImplItemConst {
                            attrs,
                            vis,
                            const_token,
                            ident,
                            colon_token: token::Colon {
                                spans: Default::default(),
                            },
                            ty,
                            eq_token: token::Eq {
                                spans: Default::default(),
                            },
                            expr,
                            semi_token: token::Semi::default(),
                            defaultness: None,
                            generics: Default::default(),
                        });
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant values with non-constant values, potentially triggering bugs related to generic const expressions, associated const equality, and other constant contexts."
    }
}