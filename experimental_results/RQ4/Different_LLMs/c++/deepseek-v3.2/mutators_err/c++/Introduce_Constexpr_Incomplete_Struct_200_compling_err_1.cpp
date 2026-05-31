//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Constexpr_Incomplete_Struct_200
 */ 
class MutatorFrontendAction_200 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(200)

private:
    class MutatorASTConsumer_200 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_200(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::RecordDecl*> incompleteStructs;
        std::set<std::string> existingNames;
        bool namesCollected = false;
    };
};

//source file
#include "../include/Introduce_Constexpr_Incomplete_Struct_200.h"

// ========================================================================================================
#define MUT200_OUTPUT 1

void MutatorFrontendAction_200::Callback::run(const MatchFinder::MatchResult &Result) {
    if (!namesCollected) {
        if (auto *ND = Result.Nodes.getNodeAs<clang::NamedDecl>("NamedDecl")) {
            if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(ND->getLocation()))
                return;
            if (ND->getIdentifier()) {
                existingNames.insert(ND->getNameAsString());
            }
        }
        return;
    }

    if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("IncompleteStruct")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (!RD->getIdentifier())
            return;
        if (!RD->isCompleteDefinition()) {
            incompleteStructs.push_back(RD);
        }
    }
}
  
void MutatorFrontendAction_200::MutatorASTConsumer_200::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    Callback callback(TheRewriter);
    
    DeclarationMatcher nameMatcher = namedDecl().bind("NamedDecl");
    matchFinder.addMatcher(nameMatcher, &callback);
    matchFinder.matchAST(Context);
    
    callback.namesCollected = true;
    
    MatchFinder structFinder;
    DeclarationMatcher structMatcher = recordDecl().bind("IncompleteStruct");
    structFinder.addMatcher(structMatcher, &callback);
    structFinder.matchAST(Context);
    
    SourceManager &SM = TheRewriter.getSourceMgr();
    std::string newVarName = "mut_op_var_200";
    int counter = 0;
    while (callback.existingNames.find(newVarName) != callback.existingNames.end()) {
        newVarName = "mut_op_var_200_" + std::to_string(counter++);
    }
    
    std::string structName;
    bool needForwardDecl = false;
    
    if (!callback.incompleteStructs.empty()) {
        size_t idx = getrandom::getRandomIndex(callback.incompleteStructs.size() - 1);
        const RecordDecl* chosen = callback.incompleteStructs[idx];
        structName = chosen->getNameAsString();
    } else {
        structName = "MutOpStruct_200";
        needForwardDecl = true;
    }
    
    std::string insertion;
    if (needForwardDecl) {
        insertion = "struct " + structName + ";\n";
    }
    insertion += "constexpr struct " + structName + " " + newVarName + " = { 0 };\n";
    insertion = "/*mut200*/" + insertion;
    
    SourceLocation insertLoc = SM.getLocForStartOfFile(SM.getMainFileID());
    TheRewriter.InsertText(insertLoc, insertion);
}