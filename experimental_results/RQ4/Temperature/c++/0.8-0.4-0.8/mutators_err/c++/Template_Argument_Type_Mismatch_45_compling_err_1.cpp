//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_argument_type_mismatch_45
 */ 
class MutatorFrontendAction_45 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(45)

private:
    class MutatorASTConsumer_45 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_45(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_argument_type_mismatch_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TSD = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TSD->getBeginLoc()))
        return;

      // Get the source code text of target node
      auto templateArgs = TSD->template_arguments();
      if (templateArgs.size() > 0) {
          auto firstArg = templateArgs[0];
          if (firstArg.getKind() == TemplateArgument::Type) {
              auto originalType = firstArg.getAsType().getAsString();
              std::string mutatedType = originalType + "&"; // Introduce reference type

              // Perform mutation on the source code text by applying string replacement
              auto sourceRange = TSD->getSourceRange();
              std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
              std::string mutatedText = originalText;
              size_t pos = mutatedText.find(originalType);
              if (pos != std::string::npos) {
                  mutatedText.replace(pos, originalType.length(), mutatedType);
              }

              // Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
          }
      }
    }
}
  
void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}