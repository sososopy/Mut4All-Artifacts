//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * misplace_template_parameter_29
 */ 
class MutatorFrontendAction_29 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(29)

private:
    class MutatorASTConsumer_29 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_29(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TemplateDecl *TargetTemplateDecl = nullptr;
    };
};

//source file
#include "../include/misplace_template_parameter_29.h"

// ========================================================================================================
#define MUT29_OUTPUT 1

void MutatorFrontendAction_29::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
        return;

      TargetTemplateDecl = TD;
      auto &Context = *Result.Context;
      auto &SM = Context.getSourceManager();
      auto &LO = Context.getLangOpts();
      
      std::string TemplateText = Lexer::getSourceText(CharSourceRange::getTokenRange(TD->getSourceRange()), SM, LO).str();

      if (!TemplateText.empty()) {
        // Remove template parameter from original location
        std::string::size_type pos = TemplateText.find("<");
        std::string::size_type endPos = TemplateText.find(">");
        if (pos != std::string::npos && endPos != std::string::npos) {
          std::string TemplateParams = TemplateText.substr(pos, endPos - pos + 1);
          TemplateText.erase(pos, endPos - pos + 1);

          // Insert template parameters in a nonsensical place
          TemplateText += "\nvoid misplacedFunction() {\n" + TemplateParams + " int x = 0; }\n";
          
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), TemplateText);
        }
      }
    }
}
  
void MutatorFrontendAction_29::MutatorASTConsumer_29::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}