//header file
#pragma once
#include "Mutator_base.h"

/**
 * Simulate_Constexpr_Memory_Allocation_462
 */ 
class MutatorFrontendAction_462 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(462)

private:
    class MutatorASTConsumer_462 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_462(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/simulate_constexpr_memory_allocation_462.h"

// ========================================================================================================
#define MUT462_OUTPUT 1

void MutatorFrontendAction_462::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ConstexprFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      if (FD->isConstexpr() && FD->hasBody()) {
        //Get the source code text of target node
        auto body = stringutils::rangetoStr(*(Result.SourceManager),
                                            FD->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string mutation = "for (int i = 0; i < 100; ++i) { vec.push_back(value); }";
        size_t pos = body.find("vec.push_back(value);");
        if (pos != std::string::npos) {
            body.replace(pos, std::string("vec.push_back(value);").length(), mutation);
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), body);
      }
    }
}
  
void MutatorFrontendAction_462::MutatorASTConsumer_462::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isConstexpr(), hasDescendant(callExpr(callee(functionDecl(hasName("push_back")))))).bind("ConstexprFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// Mutation information
void chain_of_thought() {
    // The mutation targets the use of `constexpr` with data structures that involve dynamic memory operations during compile time.
    // Specifically, it focuses on operations like `insert`, `emplace`, and `push_back` in `constexpr` contexts with standard containers like `std::vector`.
    // The mutation simulates conditions that might lead to incorrect memory handling or allocations during constant evaluation.
    // It replaces single container operations with a loop structure to aggressively test memory handling.
}