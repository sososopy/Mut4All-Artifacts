use crate::mutator::Mutator;
use syn::parse_quote;
use syn::Item;
use syn::Stmt;
use syn::Pat;
use syn::LocalInit;
use syn::PatWild;
use syn::PatType;
use syn::token::Colon;
use syn::Expr;
use syn::ExprReference;
use syn::token::And;

pub struct Replace_Var_Binding_With_ImplTrait_409;

impl Mutator for Replace_Var_Binding_With_ImplTrait_409 {
    fn name(&self) -> &str {
        "Replace_Var_Binding_With_ImplTrait_409"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(ref mut local) = stmt {
                        if let Pat::Ident(_) = **local.pat {
                            if let Some(LocalInit { expr, .. }) = &mut local.init {
                                let new_type = parse_quote! { impl PartialEq };
                                let original_pat = std::mem::replace(&mut local.pat, Box::new(Pat::Wild(PatWild { underscore_token: syn::token::Underscore::default() })));
                                let new_pat = PatType {
                                    attrs: vec![],
                                    pat: original_pat,
                                    colon_token: Colon::default(),
                                    ty: Box::new(new_type),
                                };
                                local.pat = Box::new(Pat::Type(new_pat));
                                *expr = Box::new(Expr::Reference(ExprReference {
                                    attrs: vec![],
                                    and_token: And::default(),
                                    expr: Box::new(**expr.clone()),
                                    mutability: None,
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces variable bindings with opaque `impl Trait` type annotations using existing traits, inserting a reference to `impl Trait`. This transformation forces the compiler to handle opaque types in bindings, potentially exposing issues in type inference, HIR construction, or trait resolution when type information is intentionally obfuscated."
    }
}