//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Type_Parameter_With_Concept_Type_92
 */ 
class MutatorFrontendAction_92 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(92)

private:
    class MutatorASTConsumer_92 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_92(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Type_Parameter_With_Concept_Type_92.h"

// ========================================================================================================
#define MUT92_OUTPUT 1

void MutatorFrontendAction_92::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      for (const auto &param : MT->getTemplateParameters()->asArray()) {
        if (const auto *typeParam = dyn_cast<TemplateTypeParmDecl>(param)) {
          // Find concept types in the code
          for (const auto &decl : MT->getDeclContext()->decls()) {
            if (const auto *conceptDecl = dyn_cast<ConceptDecl>(decl)) {
              // Replace type parameter with concept type
              std::string conceptName = conceptDecl->getNameAsString();
              templateDecl.replace(templateDecl.find(typeParam->getNameAsString()), 
                                  typeParam->getNameAsString().length(), conceptName);
            }
          }
        }
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), templateDecl);
    }
}
  
void MutatorFrontendAction_92::MutatorASTConsumer_92::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}