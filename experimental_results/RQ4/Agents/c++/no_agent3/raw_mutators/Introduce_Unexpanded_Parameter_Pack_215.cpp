//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Unexpanded_Parameter_Pack_215
 */ 
class MutatorFrontendAction_215 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(215)

private:
    class MutatorASTConsumer_215 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_215(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/introduce_unexpanded_parameter_pack_215.h"

// ========================================================================================================
#define MUT215_OUTPUT 1

void MutatorFrontendAction_215::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      if (FT->getTemplatedDecl()->hasBody()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string insertion = R"(
        auto unused_pack = [&](auto... unused) {
            (void)sizeof...(unused);
        };
        unused_pack();
        )";
        size_t pos = declaration.find("{");
        if (pos != std::string::npos) {
            declaration.insert(pos + 1, insertion);
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_215::MutatorASTConsumer_215::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(packExpansionExpr())).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}