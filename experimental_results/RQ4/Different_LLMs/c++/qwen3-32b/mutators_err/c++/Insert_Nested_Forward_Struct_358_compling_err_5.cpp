//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Nested_Forward_Struct_358
 */ 
class MutatorFrontendAction_358 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(358)

private:
    class MutatorASTConsumer_358 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_358(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<std::string> structNames;
    };
};

//source file
#include "../include/Insert_Nested_Forward_Struct_358.h"

// ========================================================================================================
#define MUT358_OUTPUT 1

void MutatorFrontendAction_358::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *structDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
        if (!structDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                       structDecl->getLocation()))
            return;
        structNames.push_back(structDecl->getNameAsString());
    } else if (auto *funcDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!funcDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                       funcDecl->getLocation()))
            return;
        if (structNames.empty())
            return;
        const CompoundStmt *body = dyn_cast<CompoundStmt>(funcDecl->getBody());
        if (!body)
            return;
        SourceLocation startLoc = body->getLBracLoc();
        std::string structName = structNames[getrandom::getRandomIndex(structNames.size())];
        std::string structCode = "struct " + structName + " {\n    struct " + structName + ";\n};\n";
        Rewrite.InsertTextAfter(startLoc, structCode);
    }
}
  
void MutatorFrontendAction_358::MutatorASTConsumer_358::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("Functions").or_(cxxRecordDecl().bind("Structs"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}