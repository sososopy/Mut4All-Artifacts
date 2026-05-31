use syn::{Item, ItemFn, ItemStatic, Visibility, Signature, Generics, Punctuated, ReturnType, token, Expr, ExprCall, ExprPath, ExprReturn, Stmt, Path, PathSegment, PathArguments};
use proc_macro2::Span;
use crate::mutator::Mutator;

struct Replace_Static_Variable_Initialization_With_Const_Function_Call_74;

impl Mutator for Replace_Static_Variable_Initialization_With_Const_Function_Call_74 {
    fn name(&self) -> &str {
        "Replace_Static_Variable_Initialization_With_Const_Function_Call_74"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(static_item) = item {
                let init = &static_item.expr;
                let ty = &static_item.ty;
                let ident = &static_item.ident;
                let vis = &static_item.vis;

                let const_fn_ident = syn::Ident::new(&format!("init_{}", ident), Span::call_site());
                let const_fn = Item::Fn(ItemFn {
                    attrs: vec![],
                    vis: Visibility::Inherited,
                    sig: Signature {
                        constness: None,
                        asyncness: None,
                        unsafety: None,
                        abi: None,
                        ident: const_fn_ident,
                        generics: Generics::default(),
                        inputs: Punctuated::new(),
                        variadic: None,
                        output: ReturnType::Type(ty.clone(), token::RArrow(Span::call_site())),
                        fn_token: token::Fn(Span::call_site()),
                        paren_token: Default::default(),
                    },
                    block: Box::new(syn::Block {
                        brace_token: token::Brace(Span::call_site()),
                        stmts: vec![Stmt::Expr(Expr::Return(ExprReturn {
                            attrs: vec![],
                            return_token: token::Return(Span::call_site()),
                            expr: Some(init.clone()),
                        }), Some(token::Semi(Span::call_site())))],
                    }),
                });

                file.items.push(const_fn);

                let new_init = Expr::Call(ExprCall {
                    attrs: vec![],
                    func: Box::new(Expr::Path(ExprPath {
                        attrs: vec![],
                        qself: None,
                        path: Path {
                            leading_colon: None,
                            segments: Punctuated::from_iter(vec![PathSegment {
                                ident: const_fn_ident,
                                arguments: PathArguments::None,
                            }]),
                        },
                    })),
                    paren_token: Default::default(),
                    args: Punctuated::new(),
                });

                let new_static_item = Item::Static(ItemStatic {
                    attrs: static_item.attrs.clone(),
                    vis: vis.clone(),
                    mutability: static_item.mutability.clone(),
                    ident: ident.clone(),
                    ty: ty.clone(),
                    expr: Box::new(new_init),
                    semi_token: static_item.semi_token.clone(),
                    static_token: static_item.static_token.clone(),
                    colon_token: token::Colon(Span::call_site()),
                    eq_token: token::Eq(Span::call_site()),
                });

                *item = new_static_item;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces static variable initializations with const function calls. It introduces a new const function for each static variable, which returns the original initialization value. This transformation aims to test the compiler's ability to evaluate constants correctly and handle the interactions between const functions and static variables."
    }
}