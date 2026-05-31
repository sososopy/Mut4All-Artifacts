//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Array_Initialization_with_Implicit_Type_Conversion_466
 */ 
class MutatorFrontendAction_466 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(466)

private:
    class MutatorASTConsumer_466 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_466(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        
    };
};

//source file
#include "../include/target_array_initialization_with_implicit_type_conversion_466.h"

// ========================================================================================================
#define MUT466_OUTPUT 1

void MutatorFrontendAction_466::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ArrayDecl")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType().getTypePtrOrNull() == nullptr)
      return;
    if (DL->getType().getTypePtrOrNull()->isArrayType() == false)
      return;
    if (DL->hasInit() == false)
      return;
    auto init = DL->getInit();
    if (init == nullptr)
      return;
    if (init->getType().getTypePtrOrNull() == nullptr)
      return;
    if (init->getType().getTypePtrOrNull()->isArrayType() == false)
      return;
    auto DL_type = DL->getType().getTypePtrOrNull();
    auto init_type = init->getType().getTypePtrOrNull();
    if (DL_type->isConstantArrayType() == false ||
        init_type->isConstantArrayType() == false)
      return;
    auto DL_array = dyn_cast<ConstantArrayType>(DL_type);
    auto init_array = dyn_cast<ConstantArrayType>(init_type);
    if (DL_array->getSize() != init_array->getSize())
      return;
    if (DL_array->getElementType().getTypePtrOrNull() == nullptr ||
        init_array->getElementType().getTypePtrOrNull() == nullptr)
      return;
    if (DL_array->getElementType().getTypePtrOrNull()->isStructureType() ==
            false ||
        init_array->getElementType().getTypePtrOrNull()->isIntegerType() ==
            false)
      return;
    auto DL_struct = DL_array->getElementType().getTypePtrOrNull();
    if (DL_struct->isStructureType() == false)
      return;
    auto DL_struct_type = DL_struct->getAsStructureType();
    auto DL_struct_decl = DL_struct_type->getDecl();
    if (DL_struct_decl->isCompleteDefinition() == false)
      return;
    auto fields = DL_struct_decl->fields();
    bool has_ptr = false;
    for (auto field : fields) {
      if (field->getType().getTypePtrOrNull() == nullptr)
        continue;
      if (field->getType().getTypePtrOrNull()->isPointerType())
        has_ptr = true;
    }
    if (has_ptr == false)
      return;
    auto DL_init = DL->getInit();
    if (DL_init == nullptr)
      return;
    if (DL_init->getStmtClass() != Stmt::InitListExprClass)
      return;
    auto DL_init_list = dyn_cast<InitListExpr>(DL_init);
    if (DL_init_list->getNumInits() == 0)
      return;
    auto DL_first_init = DL_init_list->getInit(0);
    if (DL_first_init == nullptr)
      return;
    if (DL_first_init->getStmtClass() != Stmt::ImplicitCastExprClass)
      return;
    auto DL_first_cast = dyn_cast<ImplicitCastExpr>(DL_first_init);
    if (DL_first_cast->getType().getTypePtrOrNull() == nullptr)
      return;
    if (DL_first_cast->getType().getTypePtrOrNull()->isStructureType() ==
        false)
      return;
    auto DL_first_sub = DL_first_cast->getSubExpr();
    if (DL_first_sub == nullptr)
      return;
    if (DL_first_sub->getStmtClass() != Stmt::IntegerLiteralClass)
      return;
    auto DL_first_int = dyn_cast<IntegerLiteral>(DL_first_sub);
    if (DL_first_int->getValue() != 0)
      return;
    auto DL_content = stringutils::rangetoStr(*(Result.SourceManager),
                                              DL->getSourceRange());
    DL_content = DL_content.substr(0, DL_content.rfind('}'));
    DL_content += "(int *)("")}";
    DL_content = "/*mut466*/" + DL_content;
    Rewrite.ReplaceText(DL->getSourceRange(), DL_content);
  }
}
  
void MutatorFrontendAction_466::MutatorASTConsumer_466::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("ArrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}