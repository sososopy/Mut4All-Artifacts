//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Variant_With_Tuple_29
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_29.h"

// ========================================================================================================
#define MUT29_OUTPUT 1

void MutatorFrontendAction_29::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("AliasDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Get the source code text of target node
      auto underlyingType = TD->getUnderlyingType();
      if (!underlyingType->isTemplateTypeParmType()) {
        auto specialization = underlyingType->getAs<TemplateSpecializationType>();
        if (specialization) {
          auto templateName = specialization->getTemplateName();
          if (templateName.getAsTemplateDecl()) {
            auto templateDecl = templateName.getAsTemplateDecl();
            if (templateDecl->getName() == "variant") {
              auto namespaceDecl = dyn_cast<NamespaceDecl>(templateDecl->getDeclContext());
              if (namespaceDecl && namespaceDecl->getName() == "std") {
                //Perform mutation on the source code text by applying string replacement
                std::string replacement = "std::tuple";
                SourceLocation startLoc = specialization->getBeginLoc();
                SourceLocation endLoc = startLoc.getLocWithOffset(std::strlen("std::variant") - 1);
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(SourceRange(startLoc, endLoc), replacement);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_29::MutatorASTConsumer_29::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl().bind("AliasDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}