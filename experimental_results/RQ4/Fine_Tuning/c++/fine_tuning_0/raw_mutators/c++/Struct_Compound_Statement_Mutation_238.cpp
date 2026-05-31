//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Compound_Statement_Mutation_238
 */ 
class MutatorFrontendAction_238 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(238)

private:
    class MutatorASTConsumer_238 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_238(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Struct_Compound_Statement_Mutation_238.h"

// ========================================================================================================
#define MUT238_OUTPUT 1

void MutatorFrontendAction_238::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isStruct() || MT->isUnion()) {
        auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                  MT->getSourceRange());
        llvm::outs() << "/*mut238*/" << definition << '\n';
        definition.insert(definition.find("{") + 1, "{ ");
        definition.insert(definition.rfind("}"), " }");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), definition);
      }
    }
}
  
void MutatorFrontendAction_238::MutatorASTConsumer_238::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}