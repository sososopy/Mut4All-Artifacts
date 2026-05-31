//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Alias_With_Constant_Expression_375
 */ 
class MutatorFrontendAction_375 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(375)

private:
    class MutatorASTConsumer_375 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_375(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Alias_With_Constant_Expression_375.h"

// ========================================================================================================
#define MUT375_OUTPUT 1

void MutatorFrontendAction_375::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TemplateAlias")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;

      //Get the source code text of target node
      auto aliasDecl = TA->getTemplatedDecl();
      if (auto *aliasType = aliasDecl->getUnderlyingType().getTypePtrOrNull()) {
        if (auto *templateSpecialization = aliasType->getAs<clang::TemplateSpecializationType>()) {
          if (templateSpecialization->getNumArgs() > 0) {
            auto firstArg = templateSpecialization->getArg(0);
            if (firstArg.getKind() == clang::TemplateArgument::Type) {
              //Perform mutation on the source code text by applying string replacement
              std::string replacement = "typename " + aliasDecl->getNameAsString() + "::value";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(aliasDecl->getSourceRange(), replacement);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_375::MutatorASTConsumer_375::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl(hasDescendant(typeAliasDecl(hasType(templateSpecializationType())))).bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}