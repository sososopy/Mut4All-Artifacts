//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Argument_Type_Mismatch_338
 */ 
class MutatorFrontendAction_338 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(338)

private:
    class MutatorASTConsumer_338 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_338(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TemplateArgument *template_argument;
    };
};

//source file
#include "../include/template_argument_type_mismatch_338.h"

// ========================================================================================================
#define MUT338_OUTPUT 1

void MutatorFrontendAction_338::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("Template")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      auto args = MT->template_arguments();
      std::vector<int> candidate;
      for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i].getKind() == TemplateArgument::Type) {
          candidate.push_back(i);
        }
      }
      if (candidate.empty())
        return;
      int index = getrandom::getRandomIndex(candidate.size() - 1);
      template_argument = &args[candidate[index]];
      auto arg_str = stringutils::rangetoStr(*(Result.SourceManager),
                                             template_argument->getSourceRange());
      llvm::outs() << "mut338: " << arg_str << '\n';
      std::string replacement = "1";
      Rewrite.ReplaceText(template_argument->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_338::MutatorASTConsumer_338::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = type().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}