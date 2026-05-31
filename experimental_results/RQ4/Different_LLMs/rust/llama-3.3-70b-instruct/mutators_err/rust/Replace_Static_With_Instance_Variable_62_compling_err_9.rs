use syn::{File, Item, Pat, PatIdent, token, Expr, Stmt, Lit, ExprLet, Visibility};
use quote::spanned::Spanned;

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
                let let_stmt = Stmt::Expr(Expr::Let(ExprLet {
                    attrs: static_item.attrs.clone(),
                    let_token: token::Let {
                        span: static_item.vis.span(),
                    },
                    pat: Box::new(pat),
                    eq_token: static_item.eq_token,
                    expr: static_item.expr.clone(),
                }), Some(token::Semi { spans: static_item.semi_token.spans }));
                let new_item = Item::Stmt(Stmt::Expr(Expr::Let(ExprLet {
                    attrs: static_item.attrs.clone(),
                    let_token: token::Let {
                        span: static_item.vis.span(),
                    },
                    pat: Box::new(pat),
                    eq_token: static_item.eq_token,
                    expr: static_item.expr.clone(),
                })));
                *item = new_item;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces static variables with instance variables to test the Rust compiler's handling of const evaluation and static initialization. By changing the scope and lifetime of variables, it can expose bugs related to A-const-eval and initialization order."
    }
}