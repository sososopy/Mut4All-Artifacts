//header file
#pragma once
#include "Mutator_base.h"

/**
 * ForwardDeclareIncompleteTypeApplyBuiltinDump_337
 */ 
class MutatorFrontendAction_337 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(337)

private:
    class MutatorASTConsumer_337 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_337(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::string typeName;
        std::string callbackName;
        SourceLocation functionLoc;
        bool typeFound = false;
        bool callbackFound = false;
        bool functionFound = false;
    };
};

//source file
#include "../include/ForwardDeclareIncompleteTypeApplyBuiltinDump_337.h"

// ========================================================================================================
#define MUT337_OUTPUT 1

void MutatorFrontendAction_337::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *typeDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("typeDecl")) {
        if (!typeDecl || !Result.Context->getSourceManager().isWrittenInMainFile(typeDecl->getLocation()))
            return;
        if (!typeDecl->isCompleteDefinition())
            return;
        typeName = typeDecl->getNameAsString();
        typeFound = true;
    } else if (auto *funcDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
        if (!funcDecl || !Result.Context->getSourceManager().isWrittenInMainFile(funcDecl->getLocation()))
            return;
        if (!funcDecl->hasBody())
            return;
        functionLoc = funcDecl->getBody()->getBeginLoc();
        functionFound = true;
    } else if (auto *callbackDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("callbackDecl")) {
        if (!callbackDecl || !Result.Context->getSourceManager().isWrittenInMainFile(callbackDecl->getLocation()))
            return;
        if (callbackDecl->getReturnType()->isVoidType() && 
            callbackDecl->getNumParams() == 2 &&
            callbackDecl->getParamDecl(0)->getType()->isPointerType() &&
            callbackDecl->getParamDecl(0)->getType()->getAs<clang::PointerType>()->getPointeeType()->isVoidType() &&
            callbackDecl->getParamDecl(1)->getType()->getAs<clang::BuiltinType>() && 
            callbackDecl->getParamDecl(1)->getType()->getAs<clang::BuiltinType>()->getKind() == clang::BuiltinType::Size_T) {
            callbackName = callbackDecl->getNameAsString();
            callbackFound = true;
        }
    }

    if (typeFound && functionFound && callbackFound) {
        std::string forwardDecl = "struct " + typeName + ";";
        Rewrite.InsertTextBefore(functionLoc, forwardDecl + "\n");
        std::string codeToInsert = typeName + "* ptr = nullptr;\n__builtin_dump_struct(ptr, " + callbackName + ");\n";
        Rewrite.InsertText(functionLoc, codeToInsert);
    }
}

void MutatorFrontendAction_337::MutatorASTConsumer_337::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher typeMatcher = cxxRecordDecl(isDefinition()).bind("typeDecl");
    DeclarationMatcher functionMatcher = functionDecl(hasBody(compoundStmt())).bind("functionDecl");
    DeclarationMatcher callbackMatcher = functionDecl(
        returns(voidType()),
        hasParameter(0, hasType(pointerType(pointee(voidType())))),
        hasParameter(1, hasType(builtinType()))
    ).bind("callbackDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(callbackMatcher, &callback);
    matchFinder.matchAST(Context);
}