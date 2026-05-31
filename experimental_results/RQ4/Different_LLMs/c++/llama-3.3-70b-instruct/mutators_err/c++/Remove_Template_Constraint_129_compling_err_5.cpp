//header file
#pragma once
#include "Mutator_base.h"
#include <regex>

/**
 * Remove_Template_Constraint_129
 */ 
class MutatorFrontendAction_Remove_Template_Constraint_129 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Remove_Template_Constraint_129)

private:
    class MutatorASTConsumer_Remove_Template_Constraint_129 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Remove_Template_Constraint_129(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Remove_Template_Constraint_129.h"

// ========================================================================================================
#define REMOVE_TEMPLATE_CONSTRAINT_129_OUTPUT 1

void MutatorFrontendAction_Remove_Template_Constraint_129::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = llvm::StringRef::withNullAsEmpty(stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange()));
      //Perform mutation on the source code text by applying string replacement
      std::regex pattern("requires\\s+[^{}]+");
      declaration = std::regex_replace(declaration.str(), pattern, "");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_Remove_Template_Constraint_129::MutatorASTConsumer_Remove_Template_Constraint_129::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}