//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Template_Instantiation_And_Overloaded_Operators_478
 */ 
class MutatorFrontendAction_478 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(478)

private:
    class MutatorASTConsumer_478 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_478(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionTemplateDecl* CurrentTemplate = nullptr;
    };
};

//source file
#include "../include/modify_function_template_instantiation_and_overloaded_operators_478.h"

// ========================================================================================================
#define MUT478_OUTPUT 1

void MutatorFrontendAction_478::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        //Filter nodes in header files
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                        FTD->getLocation()))
            return;
        
        CurrentTemplate = FTD;

        // Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FTD->getSourceRange());

        // Introduce a new type T3
        std::string newType = "\nstruct NewType { int value; };";
        declaration.insert(declaration.find("{") + 1, newType);

        // Introduce a new overloaded operator
        std::string newOperator = "\ntemplate<typename T1>\n"
                                  "T1 operator|(const T1& lhs, const NewType& rhs) {\n"
                                  "    return lhs; // Simple implementation\n"
                                  "}";
        declaration.insert(declaration.find("}"), newOperator);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_478::MutatorASTConsumer_478::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(binaryOperator(hasOperatorName("|")))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}