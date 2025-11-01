use syn::parse_quote;
use syn::Item;
use syn::punctuated::Punctuated;
use syn::FnArg;
use syn::Type;
use syn::Expr;
use syn::Ident;
use proc_macro2::Span;
use syn::PatType;
use crate::mutator::Mutator;

pub struct Modify_Array_Length_172;

impl Mutator for Modify_Array_Length_172 {
    fn name(&self) -> &str {
        "Modify_Array_Length_172"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_inputs = Punctuated::new();
                let mut added_len_param = false;

                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Array(type_array) = &*pat_type.ty {
                            if let Expr::Lit(expr_lit) = &type_array.len {
                                if let syn::Lit::Int(_) = expr_lit.lit {
                                    let new_ident = Ident::new("len", Span::call_site());
                                    let new_param: FnArg = parse_quote!(#new_ident: usize);

                                    let new_array_type: Type = parse_quote!([#type_array.elem; #new_ident]);
                                    let new_pat_type = PatType {
                                        ty: Box::new(new_array_type),
                                        attrs: pat_type.attrs.clone(),
                                        pat: pat_type.pat.clone(),
                                        colon_token: pat_type.colon_token,
                                    };

                                    if !added_len_param {
                                        new_inputs.push(new_param);
                                        added_len_param = true;
                                    }
                                    new_inputs.push(FnArg::Typed(new_pat_type));
                                    continue;
                                }
                            }
                        }
                    }
                    new_inputs.push(input.clone());
                }
                item_fn.sig.inputs = new_inputs;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator modifies function parameters that are arrays with constant lengths. It changes the constant length to a new function parameter, introducing dependency on a parameter that might not be evaluated correctly. This mutation can lead to unexpected behavior, especially under experimental features, as it challenges the compiler's type inference and evaluation mechanisms."
    }
}