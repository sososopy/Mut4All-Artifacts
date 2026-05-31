use syn::{File, Item, Pat, PatIdent, token, Expr, Stmt, Lit};
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
                let init = Some((token::Eq { spans: static_item.eq_token.spans }, static_item.expr.clone()));
                let semi_token = static_item.semi_token;
                let let_stmt = Stmt::Expr(Expr::Let(syn::ExprLet {
                    attrs: static_item.attrs.clone(),
                    let_token: token::Let {
                        span: static_item.vis.span(),
                    },
                    pat: Box::new(pat),
                    init,
                    semi_token,
                }));
                let new_item = Item::Stmt(let_stmt);
                *item = new_item;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces static variables with instance variables to test the Rust compiler's handling of const evaluation and static initialization. By changing the scope and lifetime of variables, it can expose bugs related to A-const-eval and initialization order."
    }
}