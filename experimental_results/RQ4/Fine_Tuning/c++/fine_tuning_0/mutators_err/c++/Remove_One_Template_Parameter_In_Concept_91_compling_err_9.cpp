//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_One_Template_Parameter_In_Concept_91
 */ 
class MutatorFrontendAction_91 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(91)

private:
    class MutatorASTConsumer_91 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_91(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/remove_one_template_parameter_in_concept_91.h"

// ========================================================================================================
#define MUT91_OUTPUT 1

void MutatorFrontendAction_91::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      llvm::outs() << content;
      auto TP = DL->getTemplateParameters();
      if (TP->size() <= 1)
        return;
      auto first_param = TP->getParam(0);
      auto second_param = TP->getParam(1);
      auto first_param_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                     first_param->getSourceRange());
      auto second_param_str =
          stringutils::rangetoStr(*(Result.SourceManager),
                                  second_param->getSourceRange());
      llvm::outs() << first_param_str << ' ' << second_param_str;
      auto first_pos = content.find(first_param_str);
      auto second_pos = content.find(second_param_str);
      if (first_pos == string::npos || second_pos == string::npos)
        return;
      content.erase(second_pos, second_param_str.length());
      llvm::outs() << content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_91::MutatorASTConsumer_91::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}