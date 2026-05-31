use syn::{File, Item, ItemConst, ItemFn, ItemImpl, ImplItem, ImplItemConst, ImplItemFn, Signature, ReturnType, punctuated::Punctuated};
use crate::mutator::Mutator;

struct Replace_Const_With_Non_Const_315;

impl Mutator for Replace_Const_With_Non_Const_315 {
    fn name(&self) -> &str {
        "Replace_Const_With_Non_Const_315"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    func.sig.constness = None;
                }
            }

            if let Item::Const(const_item) = item {
                let mut new_item = Item::Fn(ItemFn {
                    attrs: const_item.attrs.clone(),
                    vis: const_item.vis.clone(),
                    sig: Signature {
                        constness: None,
                        asyncness: None,
                        unsafety: None,
                        abi: None,
                        fn_token: Default::default(),
                        ident: const_item.ident.clone(),
                        generics: const_item.generics.clone(),
                        inputs: Punctuated::new(),
                        variadic: None,
                        output: ReturnType::Default,
                        paren_token: Default::default(),
                    },
                    block: syn::Block {
                        brace_token: Default::default(),
                        stmts: {
                            let mut stmts = Vec::new();
                            stmts.push(syn::Stmt::Expr(
                                syn::Expr::Path(syn::ExprPath {
                                    attrs: vec![],
                                    qself: None,
                                    path: match const_item.expr {
                                        syn::Expr::Path(path) => path.path.clone(),
                                        _ => panic!("Expected path"),
                                    },
                                }),
                                None,
                            ));
                            stmts
                        },
                    },
                });
                *item = new_item;
            }

            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Const(const_item) = impl_item {
                        let mut new_item = ImplItem::Fn(ImplItemFn {
                            attrs: const_item.attrs.clone(),
                            vis: const_item.vis.clone(),
                            defaultness: None,
                            sig: Signature {
                                constness: None,
                                asyncness: None,
                                unsafety: None,
                                abi: None,
                                fn_token: Default::default(),
                                ident: const_item.ident.clone(),
                                generics: const_item.generics.clone(),
                                inputs: Punctuated::new(),
                                variadic: None,
                                output: ReturnType::Default,
                                paren_token: Default::default(),
                            },
                            block: syn::Block {
                                brace_token: Default::default(),
                                stmts: {
                                    let mut stmts = Vec::new();
                                    stmts.push(syn::Stmt::Expr(
                                        syn::Expr::Path(syn::ExprPath {
                                            attrs: vec![],
                                            qself: None,
                                            path: match const_item.expr {
                                                syn::Expr::Path(path) => path.path.clone(),
                                                _ => panic!("Expected path"),
                                            },
                                        }),
                                        None,
                                    ));
                                    stmts
                                },
                            },
                        });
                        *impl_item = new_item;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const functions and variables with their non-const counterparts to check for potential issues with const correctness and evaluate how the Rust compiler handles such changes."
    }
}