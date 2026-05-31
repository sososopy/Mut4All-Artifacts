//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Struct_Member_Function_Return_Type_241
 */ 
class MutatorFrontendAction_241 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(241)

private:
    class MutatorASTConsumer_241 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_241(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *cur_struct;
    };
};

//source file
#include "../include/alter_struct_member_function_return_type_241.h"

// ========================================================================================================
#define MUT241_OUTPUT 1

void MutatorFrontendAction_241::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      cur_struct = ST;
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Funcs")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isStatic())
        return;
      if (FD->getParent() != cur_struct)
        return;
      auto rettype = FD->getReturnType();
      auto rettype_str = rettype.getAsString();
      llvm::outs() << rettype_str << '\n';
      if (rettype_str == "void")
        return;
      auto ins = "/*mut241*/typename Foo<UndefinedType>::type";
      Rewrite.ReplaceText(FD->getReturnTypeSourceRange(), ins);
    }
}
  
void MutatorFrontendAction_241::MutatorASTConsumer_241::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    auto func_matcher = functionDecl().bind("Funcs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}