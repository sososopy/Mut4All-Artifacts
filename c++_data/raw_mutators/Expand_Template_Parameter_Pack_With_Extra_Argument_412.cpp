//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Template_Parameter_Pack_With_Extra_Argument_412
 */ 
class MutatorFrontendAction_412 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(412)

private:
    class MutatorASTConsumer_412 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_412(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Expand_Template_Parameter_Pack_With_Extra_Argument_412.h"

// ========================================================================================================
#define MUT412_OUTPUT 1

void MutatorFrontendAction_412::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MethodTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t pos = declaration.find("TypeAlias<Ts...>");
        if (pos != std::string::npos) {
          declaration.replace(pos, 14, "TypeAlias<Ts..., U>");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_412::MutatorASTConsumer_412::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isTemplateInstantiation()).bind("MethodTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}