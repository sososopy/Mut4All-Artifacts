use syn::{parse_quote, GenericParam, Expr, Ident};
use proc_macro2::Span;
use rand::thread_rng;
use rand::Rng;
use crate::mutator::Mutator;

struct Replace_Const_Generic_Arg_With_Const_Expr_376;

impl Mutator for Replace_Const_Generic_Arg_With_Const_Expr_376 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Arg_With_Const_Expr_376"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let const_expr = self.generate_const_expr();
                            param.default = Some(const_expr);
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            let const_expr = self.generate_const_expr();
                            param.default = Some(const_expr);
                        }
                    }
                }

                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(generics) = &mut func.sig.generics {
                            for param in &mut generics.params {
                                if let GenericParam::Const(param) = param {
                                    let const_expr = self.generate_const_expr();
                                    param.default = Some(const_expr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic arguments with const expressions in function and type definitions. It aims to test the compiler's handling of const expressions in the context of const generics, potentially revealing bugs or inconsistencies in how these features are implemented."
    }
}

impl Replace_Const_Generic_Arg_With_Const_Expr_376 {
    fn generate_const_expr(&self) -> Expr {
        let mut rng = thread_rng();
        let const_expr_type = rng.gen_range(0..=2);

        match const_expr_type {
            0 => {
                // Generate a constant value
                let value: i32 = rng.gen_range(0..100);
                parse_quote! { #value }
            }
            1 => {
                // Generate an arithmetic expression
                let op = rng.gen_range(0..=1);
                let left: i32 = rng.gen_range(0..100);
                let right: i32 = rng.gen_range(0..100);

                match op {
                    0 => parse_quote! { #left + #right },
                    1 => parse_quote! { #left * #right },
                    _ => unreachable!(),
                }
            }
            2 => {
                // Generate a const function call
                let func_name = Ident::new("const_fn", Span::call_site());
                parse_quote! { #func_name() }
            }
            _ => unreachable!(),
        }
    }
}