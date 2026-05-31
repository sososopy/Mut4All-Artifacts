use syn::{GenericParam, Lifetime, Stmt, Expr, punctuated::Punctuated, token, BoundLifetimes};
use proc_macro2::Span;

pub trait Mutator {
    fn mutate(&self, file_ast: &mut syn::File);
}

struct Swap_Closure_Lifetimes_40;

impl Mutator for Swap_Closure_Lifetimes_40 {
    fn mutate(&self, file_ast: &mut syn::File) {
        for item in &mut file_ast.items {
            if let syn::Item::Fn(func) = item {
                self.mutate_closure_lifetimes(func);
            } else if let syn::Item::Impl(impl_item) = item {
                for method in &mut impl_item.items {
                    if let syn::ImplItem::Method(method) = method {
                        self.mutate_impl_item_fn(method);
                    }
                }
            }
        }
    }
}

impl Swap_Closure_Lifetimes_40 {
    fn mutate_closure_lifetimes(&self, func: &mut syn::ItemFn) {
        let mut lifetimes: Vec<Lifetime> = Vec::new();
        for param in &func.sig.generics.params {
            if let GenericParam::Lifetime(lifetime) = param {
                lifetimes.push(lifetime.lifetime.clone());
            }
        }

        for stmt in &mut func.block.stmts {
            if let Stmt::Expr(expr, _) = stmt {
                if let Expr::Closure(closure) = expr {
                    let mut closure_lifetimes: Punctuated<GenericParam, token::Comma> = Punctuated::new();
                    if let Some(bound_lifetimes) = &closure.lifetimes {
                        for lifetime in &bound_lifetimes.lifetimes {
                            closure_lifetimes.push(lifetime.clone());
                        }
                    }

                    if closure_lifetimes.len() > 1 {
                        let mut reversed_lifetimes: Punctuated<GenericParam, token::Comma> = Punctuated::new();
                        for lifetime in closure_lifetimes.iter().rev() {
                            reversed_lifetimes.push(lifetime.clone());
                        }
                        let for_token = token::For {
                            span: Span::call_site(),
                        };
                        let gt_token = token::Gt {
                            spans: [Span::call_site()],
                        };
                        let lt_token = token::Lt {
                            spans: [Span::call_site()],
                        };
                        let reversed_bound_lifetimes = BoundLifetimes {
                            for_token,
                            lifetimes: reversed_lifetimes,
                            gt_token,
                            lt_token,
                        };
                        let mut new_closure = closure.clone();
                        new_closure.lifetimes = Some(reversed_bound_lifetimes);
                        *expr = Expr::Closure(new_closure);
                    }
                }
            }
        }
    }

    fn mutate_impl_item_fn(&self, method: &mut syn::ImplItemMethod) {
        let mut lifetimes: Vec<Lifetime> = Vec::new();
        for param in &method.sig.generics.params {
            if let GenericParam::Lifetime(lifetime) = param {
                lifetimes.push(lifetime.lifetime.clone());
            }
        }

        for stmt in &mut method.block.stmts {
            if let Stmt::Expr(expr, _) = stmt {
                if let Expr::Closure(closure) = expr {
                    let mut closure_lifetimes: Punctuated<GenericParam, token::Comma> = Punctuated::new();
                    if let Some(bound_lifetimes) = &closure.lifetimes {
                        for lifetime in &bound_lifetimes.lifetimes {
                            closure_lifetimes.push(lifetime.clone());
                        }
                    }

                    if closure_lifetimes.len() > 1 {
                        let mut reversed_lifetimes: Punctuated<GenericParam, token::Comma> = Punctuated::new();
                        for lifetime in closure_lifetimes.iter().rev() {
                            reversed_lifetimes.push(lifetime.clone());
                        }
                        let for_token = token::For {
                            span: Span::call_site(),
                        };
                        let gt_token = token::Gt {
                            spans: [Span::call_site()],
                        };
                        let lt_token = token::Lt {
                            spans: [Span::call_site()],
                        };
                        let reversed_bound_lifetimes = BoundLifetimes {
                            for_token,
                            lifetimes: reversed_lifetimes,
                            gt_token,
                            lt_token,
                        };
                        let mut new_closure = closure.clone();
                        new_closure.lifetimes = Some(reversed_bound_lifetimes);
                        *expr = Expr::Closure(new_closure);
                    }
                }
            }
        }
    }
}