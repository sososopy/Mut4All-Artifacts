//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_argument_to_incomplete_type_679
 */ 
class MutatorFrontendAction_679 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(679)

private:
    class MutatorASTConsumer_679 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_679(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_argument_to_incomplete_type_679.h"

// ========================================================================================================
#define MUT679_OUTPUT 1

void MutatorFrontendAction_679::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateCall")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;

      const TemplateArgumentList *TArgs = MT->getDirectCallee()->getTemplateSpecializationArgs();
      if (!TArgs || TArgs->size() == 0) return;

      for (unsigned i = 0; i < TArgs->size(); ++i) {
        if (TArgs->get(i).getKind() == TemplateArgument::Type) {
          QualType QT = TArgs->get(i).getAsType();
          if (QT->isBuiltinType()) {
            SourceLocation Start = MT->getExprLoc();
            SourceLocation End = Lexer::getLocForEndOfToken(MT->getEndLoc(), 0, *Result.SourceManager, Result.Context->getLangOpts());
            std::string OriginalText = Lexer::getSourceText(CharSourceRange::getTokenRange(Start, End), *Result.SourceManager, Result.Context->getLangOpts()).str();
            std::string MutatedText = OriginalText;
            size_t pos = MutatedText.find(QT.getAsString());
            if (pos != std::string::npos) {
              MutatedText.replace(pos, QT.getAsString().length(), "IncompleteType");
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(Start, End), MutatedText);
            }
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_679::MutatorASTConsumer_679::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasAnyTemplateArgument(anything()))).bind("TemplateCall"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}