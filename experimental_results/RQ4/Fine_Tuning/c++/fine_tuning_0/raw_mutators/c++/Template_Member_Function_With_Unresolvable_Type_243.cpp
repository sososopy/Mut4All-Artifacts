//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Member_Function_With_Unresolvable_Type_243
 */ 
class MutatorFrontendAction_243 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(243)

private:
    class MutatorASTConsumer_243 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_243(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Template_Member_Function_With_Unresolvable_Type_243.h"

// ========================================================================================================
#define MUT243_OUTPUT 1

void MutatorFrontendAction_243::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (!CL->isCompleteDefinition() || !CL->isTemplated())
        return;
      llvm::outs() << "Mut243: Find a template class\n";
      auto decls = CL->decls();
      for (auto decl : decls) {
        if (auto func = dyn_cast<CXXMethodDecl>(decl)) {
          if (func->isTemplated()) {
            auto ret_type = func->getReturnType();
            auto ret_type_str = ret_type.getAsString();
            llvm::outs() << "Mut243: Find a template member function\n";
            llvm::outs() << "Mut243: The return type is " << ret_type_str
                         << "\n";
            string new_ret_type =
                "typename std::enable_if<false, " + ret_type_str + ">::type";
            llvm::outs() << "Mut243: The new return type is " << new_ret_type
                         << "\n";
            Rewrite.ReplaceText(func->getReturnTypeSourceRange(), new_ret_type);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_243::MutatorASTConsumer_243::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}