//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_member_function_pointer_38
 */ 
class MutatorFrontendAction_38 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(38)

private:
    class MutatorASTConsumer_38 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_38(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberFunctionPointer")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;

      auto sourceRange = MT->getSourceRange();
      auto sourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), 
                                             *Result.SourceManager, 
                                             Result.Context->getLangOpts());

      std::string mutatedText = sourceText.str();
      size_t templatePos = mutatedText.find("template");
      if (templatePos != std::string::npos) {
          mutatedText.erase(templatePos, 8); // Remove 'template' keyword
      }

      Rewrite.ReplaceText(sourceRange, mutatedText);
    }
}
  
void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = memberExpr(hasType(templateSpecializationType()), 
                              hasAncestor(cxxRecordDecl(isTemplateInstantiation())))
                              .bind("MemberFunctionPointer");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}