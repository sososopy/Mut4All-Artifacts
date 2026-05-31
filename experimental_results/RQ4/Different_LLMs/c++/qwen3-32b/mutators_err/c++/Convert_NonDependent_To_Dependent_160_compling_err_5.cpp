//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_NonDependent_To_Dependent_160
 */ 
class MutatorFrontendAction_160 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(160)

private:
    class MutatorASTConsumer_160 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_160(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Convert_NonDependent_To_Dependent_160.h"
#include "clang/AST/Type.h"
#include "clang/AST/TemplateBase.h"
#include "clang/AST/Expr.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/ASTContext.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/raw_ostream.h"

// ========================================================================================================
#define MUT160_OUTPUT 1

void MutatorFrontendAction_160::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *methodDecl = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("methodDecl")) {
        if (!methodDecl || !Result.Context->getSourceManager().isWrittenInMainFile(methodDecl->getLocation()))
            return;
        
        if (auto *conversionCall = Result.Nodes.getNodeAs<clang::CXXOperatorCallExpr>("conversionCall")) {
            auto type = conversionCall->getType();
            if (type.isNull())
                return;
            
            clang::TypeSourceInfo *typeSourceInfo = type->getTypeSourceInfo();
            if (!typeSourceInfo)
                return;
            
            TypeLoc typeLoc = typeSourceInfo->getTypeLoc();
            
            SourceRange typeRange = typeLoc.getSourceRange();
            if (typeRange.isInvalid())
                return;
            
            auto *recordDecl = methodDecl->getParent();
            QualType recordType = Result.Context->getTypeDeclType(recordDecl);
            if (auto *TST = dyn_cast<clang::TemplateSpecializationType>(recordType)) {
                if (TST->getNumArgs() < 1)
                    return;
                
                const clang::TemplateArgument &arg = TST->getArg(0);
                if (arg.getKind() != clang::TemplateArgument::Type)
                    return;
                
                clang::QualType type = arg.getAsType();
                std::string newType = type.getAsString();
                
                Rewrite.ReplaceText(typeRange, newType);
            }
        }
    }
}
  
void MutatorFrontendAction_160::MutatorASTConsumer_160::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        isDefinition(),
        ofClass(isDerivedFrom(cxxRecordDecl(isTemplateInstantiation()))),
        hasDescendant(
            cxxOperatorCallExpr(
                hasOverloadedOperatorName("operator"),
                hasType(qualType(unless(clang::ast_matchers::type::isDependentType())))
            ).bind("conversionCall")
        )
    ).bind("methodDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}