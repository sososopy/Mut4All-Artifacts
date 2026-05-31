//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Specialization_Const_235
 */ 
class MutatorFrontendAction_235 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(235)

private:
    class MutatorASTConsumer_235 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_235(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Specialization_Const_235.h"

// ========================================================================================================
#define MUT235_OUTPUT 1

void MutatorFrontendAction_235::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("Template")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto callee = MT->getMethodDecl();
      if (!callee->isFunctionTemplateSpecialization())
        return;
      auto args = MT->getArgs();
      if (args.size() == 0)
        return;
      auto arg0 = args[0];
      auto arg0_type = arg0->getType();
      if (!arg0_type.isConstQualified())
        return;
      auto arg0_type_str = arg0_type.getAsString();
      llvm::outs() << arg0_type_str << '\n';
      auto arg0_type_str_mut = arg0_type_str;
      if (arg0_type_str.find("const") != string::npos)
        arg0_type_str_mut.erase(arg0_type_str_mut.find("const"), 6);
      else
        arg0_type_str_mut.insert(0, "const ");
      llvm::outs() << arg0_type_str_mut << '\n';
      auto arg0_str =
          stringutils::rangetoStr(*(Result.SourceManager), arg0->getSourceRange());
      llvm::outs() << arg0_str << '\n';
      auto arg0_str_mut = arg0_str;
      arg0_str_mut = "(" + arg0_type_str_mut + ")" + arg0_str;
      llvm::outs() << arg0_str_mut << '\n';
      Rewrite.ReplaceText(arg0->getSourceRange(), arg0_str_mut);
    }
}
  
void MutatorFrontendAction_235::MutatorASTConsumer_235::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMemberCallExpr().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}