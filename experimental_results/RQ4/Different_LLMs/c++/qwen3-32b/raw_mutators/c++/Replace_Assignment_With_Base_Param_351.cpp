//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Assignment_With_Base_Param_351
 */ 
class MutatorFrontendAction_351 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(351)

private:
    class MutatorASTConsumer_351 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_351(Rewriter &R) : TheRewriter(R) {}
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
        // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Assignment_With_Base_Param_351.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"

// ========================================================================================================
#define MUT351_OUTPUT 1

void MutatorFrontendAction_351::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("assignmentOp")) {
        // Check if in main file
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        // Get the enclosing class
        auto *recordDecl = MT->getParent();
        if (!recordDecl)
            return;

        // Get the first base class
        std::string baseName;
        for (const auto *base : recordDecl->bases()) {
            if (base->isVirtual()) continue; // Assuming we take the first non-virtual base
            QualType baseType = base->getType();
            if (const auto *typeDecl = baseType->getAs<RecordType>()) {
                baseName = typeDecl->getDecl()->getNameAsString();
                break;
            } else if (const auto *templateType = baseType->getAs<TemplateSpecializationType>()) {
                baseName = templateType->getTemplateName().getAsTemplateDecl()->getNameAsString();
                break;
            }
        }
        if (baseName.empty())
            return;

        // Construct the new parameter
        std::string newParam = "this " + baseName + "& self";

        // Get the parameter list range including parentheses
        SourceLocation lParenLoc = MT->getLParenLoc();
        SourceLocation rParenLoc = MT->getRParenLoc();
        if (lParenLoc.isInvalid() || rParenLoc.isInvalid())
            return;

        CharSourceRange paramRange = CharSourceRange::getCharRange(lParenLoc, rParenLoc);
        std::string originalParams = Lexer::getSourceText(paramRange, *Result.SourceManager, Result.Context->getLangOpts()).str();

        // Extract the inside of the parentheses
        std::string inside = originalParams.substr(1, originalParams.size() - 2); // Remove the parentheses
        std::string newInside = newParam + ", " + inside;
        std::string newParams = "(" + newInside + ")";

        // Replace the parameter list
        Rewrite.ReplaceText(paramRange, newParams);
    }
}
  
void MutatorFrontendAction_351::MutatorASTConsumer_351::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxMethodDecl(
        isCopyAssignmentOperator(),
        hasAncestor(cxxRecordDecl(hasAnyBase(anything())))
    ).bind("assignmentOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}