use syn::{File, Item, Pat, PatIdent, token, ItemLet};
use crate::mutator::Mutator;

struct Replace_Static_With_Instance_Variable_62;

impl Mutator for Replace_Static_With_Instance_Variable_62 {
    fn name(&self) -> &str {
        "Replace_Static_With_Instance_Variable_62"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Static(static_item) = item {
                let pat = Pat::Ident(PatIdent {
                    attrs: vec![],
                    by_ref: None,
                    mutability: None,
                    ident: static_item.ident.clone(),
                    subpat: None,
                });
                let mut new_item = Item::Let(syn::ItemLet {
                    attrs: static_item.attrs.clone(),
                    let_token: token::Let {
                        span: static_item.vis.span(),
                    },
                    pat,
                    init: Some((token::Eq { spans: static_item.eq_token.spans }, static_item.expr.clone())),
                    semi_token: static_item.semi_token,
                });
                *item = new_item;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces static variables with instance variables to test the Rust compiler's handling of const evaluation and static initialization. By changing the scope and lifetime of variables, it can expose bugs related to A-const-eval and initialization order."
    }
}