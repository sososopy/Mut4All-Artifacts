use syn::{Item, Type, TypePath, Expr};

pub struct Modify_Static_Variable_Initialization_35;

impl Mutator for Modify_Static_Variable_Initialization_35 {
    fn name(&self) -> &str {
        "Modify_Static_Variable_Initialization_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(static_item) = item {
                if let Type::Path(TypePath { path: ref type_path, .. }) = *static_item.ty {
                    if type_path.segments.iter().any(|seg| seg.ident == "AtomicUsize") {
                        if let Expr::Call(ref mut expr_call) = *static_item.expr {
                            if let Some(Expr::Lit(expr_lit)) = expr_call.args.first_mut() {
                                if let syn::Lit::Int(ref mut lit_int) = expr_lit.lit {
                                    *lit_int = syn::LitInt::new("0", lit_int.span());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies static variables of type AtomicUsize and modifies their initialization expression to introduce a type mismatch by changing an integer literal to a floating-point literal. This aims to expose type-related issues during compilation."
    }
}