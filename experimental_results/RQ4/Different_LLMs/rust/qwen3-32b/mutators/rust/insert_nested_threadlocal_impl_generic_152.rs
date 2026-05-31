use proc_macro2::Span;
use quote::quote;
use syn::{Expr, ExprBlock, ExprMacro, GenericParam, ItemImpl, TypeParam, parse_quote, Type, TypeMacro};
use crate::mutator::Mutator;

pub struct Insert_Nested_ThreadLocal_Impl_Generic_152;

impl Mutator for Insert_Nested_ThreadLocal_Impl_Generic_152 {
    fn name(&self) -> &str {
        "Insert_Nested_ThreadLocal_Impl_Generic_152"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for param in &mut item_impl.generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        if let Some(default_box) = type_param.default.as_mut() {
                            if let Type::Macro(type_macro) = &mut *default_box {
                                if type_macro.mac.path.is_ident("thread_local") {
                                    let original_tokens = type_macro.mac.tokens.clone();
                                    let new_tokens = quote! {
                                        {
                                            pub static BAR: Bar = Bar { };
                                            thread_local! #original_tokens
                                        }
                                    };
                                    type_macro.mac.tokens = new_tokens;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a nested thread_local macro within a generic parameter list of an impl block. This creates a deeper macro nesting that may expose bugs in macro hygiene, type resolution, and HIR construction during type-checking. The nested macro introduces a new static variable (BAR: Bar) and wraps the original thread_local macro, potentially leading to ICEs due to unresolved types or incorrect scoping."
    }
}