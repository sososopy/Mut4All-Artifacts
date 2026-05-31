use syn::parse_quote;
use syn::Item;
use syn::punctuated::Punctuated;
use syn::FnArg;
use syn::PatType;
use syn::Type;

pub struct Modify_Function_Parameter_Type_339;

impl Mutator for Modify_Function_Parameter_Type_339 {
    fn name(&self) -> &str {
        "Modify_Function_Parameter_Type_339"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_inputs = Punctuated::new();
                for input in item_fn.sig.inputs.iter() {
                    if let FnArg::Typed(PatType { pat, ty, attrs, colon_token }) = input {
                        if let Type::Reference(type_reference) = &**ty {
                            if let Type::Slice(_) = &*type_reference.elem {
                                let box_type: Type = parse_quote! { Box<[T]> };
                                let new_input = FnArg::Typed(PatType {
                                    attrs: attrs.clone(),
                                    pat: pat.clone(),
                                    colon_token: *colon_token,
                                    ty: Box::new(box_type),
                                });
                                new_inputs.push(new_input);
                            } else {
                                new_inputs.push(input.clone());
                            }
                        } else {
                            new_inputs.push(input.clone());
                        }
                    } else {
                        new_inputs.push(input.clone());
                    }
                }
                item_fn.sig.inputs = new_inputs;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets generic functions and modifies function parameter types. Specifically, it transforms parameters that are references to slice types (e.g., &[T]) into boxed slice types (e.g., Box<[T]>). This change is intended to explore differences in behavior between references and boxed slices, particularly in the context of MIR optimizations."
    }
}