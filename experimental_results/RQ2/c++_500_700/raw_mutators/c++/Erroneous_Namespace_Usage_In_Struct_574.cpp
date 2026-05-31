//header file
#pragma once
#include "Mutator_base.h"

/**
 * erroneous_namespace_usage_in_struct_574
 */ 
class MutatorFrontendAction_574 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(574)

private:
    class MutatorASTConsumer_574 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_574(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/erroneous_namespace_usage_in_struct_574.h"

// ========================================================================================================
#define MUT574_OUTPUT 1

void MutatorFrontendAction_574::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        //Filter nodes in header files
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;
        if (!SD->isStruct())
            return;

        //Get the source code text of target node
        auto structText = stringutils::rangetoStr(*(Result.SourceManager), SD->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string erroneousNamespace = "\n/*mut574*/ namespace using ::undefinedNamespace;\n";
        std::string erroneousUsage = "memberFunction ( undefinedNamespace";

        if (structText.rfind('}') != std::string::npos) {
            size_t insertPos = structText.rfind('}');
            structText.insert(insertPos, erroneousNamespace + erroneousUsage);
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(SD->getSourceRange()), structText);
    }
}

void MutatorFrontendAction_574::MutatorASTConsumer_574::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}