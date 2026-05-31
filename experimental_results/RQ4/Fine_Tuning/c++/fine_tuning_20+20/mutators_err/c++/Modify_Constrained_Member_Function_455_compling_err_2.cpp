//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Constrained_Member_Function_455
 */ 
class MutatorFrontendAction_455 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(455)

private:
    class MutatorASTConsumer_455 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_455(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/modify_constrained_member_function_455.h"

// ========================================================================================================
#define MUT455_OUTPUT 1

void MutatorFrontendAction_455::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      cur_classes.push_back(ST);
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isInIdentifierNamespace(Decl::IDNS_Ordinary))
        return;
      if (!FD->isTemplated())
        return;
      if (!FD->isFunctionTemplateSpecialization())
        return;
      if (!FD->hasBody())
        return;
      if (!FD->getBody())
        return;
      if (!FD->getBody()->getType())
        return;

      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      if (FD->getBody()->getType()->isDependentType()) {
        llvm::outs() << content << '\n';
        if (content.find("requires") != string::npos) {
          auto ins = content.substr(0, content.find("requires")) + "{}\n";
          llvm::outs() << ins << '\n';
          Rewrite.ReplaceText(FD->getBeginLoc(), 0, "/*mut455*/" + ins);
        }
      }
    }
}
  
void MutatorFrontendAction_455::MutatorASTConsumer_455::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Structs");
    auto func_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}