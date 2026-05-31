//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Typename_For_Alias_Template_311
 */ 
class MutatorFrontendAction_311 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(311)

private:
    class MutatorASTConsumer_311 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_311(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Typename_For_Alias_Template_311.h"

// ========================================================================================================
#define MUT311_OUTPUT 1

void MutatorFrontendAction_311::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("Alias")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto alias = stringutils::rangetoStr(*(Result.SourceManager),
                                           MT->getSourceRange());
      auto alias_name = MT->getTemplatedDecl();
      if (!alias_name)
        return;
      auto alias_decl = alias_name->getType().getTypePtr();
      if (!alias_decl)
        return;
      if (alias_decl->getTypeClass() != clang::Type::TypeClass::TemplateSpecialization)
        return;
      //Perform mutation on the source code text by applying string replacement
      alias = "/*mut311*/" + alias;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), alias);
    }
}
  
void MutatorFrontendAction_311::MutatorASTConsumer_311::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varTemplateDecl(hasType(qualType(hasDescendant(templateSpecializationType())))).bind("Alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}