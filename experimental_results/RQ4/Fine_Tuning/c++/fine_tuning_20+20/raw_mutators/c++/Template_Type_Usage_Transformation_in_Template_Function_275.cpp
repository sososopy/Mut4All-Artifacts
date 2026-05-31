//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Type_Usage_Transformation_in_Template_Function_275
 */ 
class MutatorFrontendAction_275 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(275)

private:
    class MutatorASTConsumer_275 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_275(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Type_Usage_Transformation_in_Template_Function_275.h"

// ========================================================================================================
#define MUT275_OUTPUT 1

void MutatorFrontendAction_275::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ReturnStmt>("TemplateReturn")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << content << '\n';
      string target = "typename";
      if (content.find(target) == string::npos)
        return;
      string replacement = "template";
      content.replace(content.find(target), target.length(), replacement);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_275::MutatorASTConsumer_275::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = returnStmt(hasReturnValue(hasDescendant(
                                        dependentNameType(hasKeyword(TK_typename)))))
                                    .bind("TemplateReturn");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}