//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Instantiation_115
 */ 
class MutatorFrontendAction_115 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(115)
private:
    class MutatorASTConsumer_115 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_115(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> instantiatedTemplates;
    };
};

//source file
#include "../include/insert_unused_template_instantiation_115.h"

// ========================================================================================================
#define MUT115_OUTPUT 1

void MutatorFrontendAction_115::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      // Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation()) {
        std::string templateName = FD->getNameAsString();
        if (instantiatedTemplates.find(templateName) == instantiatedTemplates.end()) {
          instantiatedTemplates.insert(templateName);
          // Get the source code text of target node
          auto functionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
          // Perform mutation on the source code text by applying string replacement
          std::string instantiationText = "/*mut115*/ template void " + templateName + "<int>();\n";
          // Replace the original AST node with the mutated one
          Rewrite.InsertTextAfterToken(FD->getEndLoc(), instantiationText);
        }
      }
    }
}
  
void MutatorFrontendAction_115::MutatorASTConsumer_115::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}