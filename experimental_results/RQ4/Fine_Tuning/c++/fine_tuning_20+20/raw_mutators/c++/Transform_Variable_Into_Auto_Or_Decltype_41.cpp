//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Variable_Into_Auto_Or_Decltype_41
 */ 
class MutatorFrontendAction_41 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(41)

private:
    class MutatorASTConsumer_41 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_41(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> cur_vars;
    };
};

//source file
#include "../include/transform_variable_into_auto_or_decltype_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Variables")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->isLocalVarDecl() == false)
        return;
      cur_vars.push_back(VD);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>( "Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isThisDeclarationADefinition() == false)
        return;
      if (cur_vars.empty())
        return;
      size_t index = getrandom::getRandomIndex(cur_vars.size() - 1);
      auto target = cur_vars[index];
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), target->getSourceRange());
      if (target->hasInit()) {
        if (getrandom::coinToss())
          content = "auto " + target->getNameAsString() + " =" +
                    stringutils::rangetoStr(*(Result.SourceManager),
                                            target->getInit()->getSourceRange());
        else
          content = "decltype(auto) " + target->getNameAsString() + " =" +
                    stringutils::rangetoStr(*(Result.SourceManager),
                                            target->getInit()->getSourceRange());
      } else {
        if (getrandom::coinToss())
          content = "auto " + target->getNameAsString() + " = 10";
        else
          content = "decltype(auto) " + target->getNameAsString() + " = 10";
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(target->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto var_matcher = varDecl().bind("Variables");
    auto func_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}