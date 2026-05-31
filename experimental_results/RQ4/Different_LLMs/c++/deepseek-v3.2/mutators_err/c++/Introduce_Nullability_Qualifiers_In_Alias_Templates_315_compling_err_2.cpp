//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Nullability_Qualifiers_In_Alias_Templates_315
 */ 
class MutatorFrontendAction_315 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(315)

private:
    class MutatorASTConsumer_315 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_315(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Introduce_Nullability_Qualifiers_In_Alias_Templates_315.h"

// ========================================================================================================
#define MUT315_OUTPUT 1

void MutatorFrontendAction_315::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasDecl = MT->getTemplatedDecl();
      if (!aliasDecl) return;
      auto underlyingType = aliasDecl->getUnderlyingType();
      // Check if the underlying type is a simple type parameter (not already qualified)
      if (underlyingType.getTypePtr()->isTemplateTypeParmType()) {
        // Check if the alias template has exactly one template parameter
        auto templateDecl = MT->getTemplatedDecl();
        if (templateDecl && MT->getTemplateParameters()->size() == 1) {
          // Check if the underlying type is directly the template parameter (not a reference, pointer, etc.)
          // For simplicity, we assume it's a direct mapping
          // Perform mutation on the source code text by applying string replacement
          auto sourceRange = aliasDecl->getSourceRange();
          auto originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
          // Find the position of the '=' sign and the type after it
          size_t eqPos = originalText.find('=');
          if (eqPos != std::string::npos) {
            // Insert _Nullable after the2 type (before any trailing semicolon)
            size_t typeEnd = originalText.find(';', eqPos);
            if (typeEnd != std::string::npos) {
              std::string mutatedText = original2Text;
              mutatedText.insert(typeEnd, " _Nullable");
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_315::MutatorASTConsumer_315::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}