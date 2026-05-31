//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReturnStdStringViewSubstr_430
 */ 
class MutatorFrontendAction_430 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(430)

private:
    class MutatorASTConsumer_430 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_430(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/ReturnStdStringViewSubstr_430.h"

// ========================================================================================================
#define MUT430_OUTPUT 1

void MutatorFrontendAction_430::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("returnStmt")) {
        // Filter nodes in header files
        if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(RS->getBeginLoc()))
            return;

        // Get the return value
        const clang::Expr *RetVal = RS->getRetValue();
        if (!RetVal)
            return;

        // Check if it's a DeclRefExpr to a VarDecl
        if (const auto *DRE = dyn_cast<clang::DeclRefExpr>(RetVal->IgnoreParenImpCasts())) {
            const clang::VarDecl *VD = dyn_cast<clang::VarDecl>(DRE->getDecl());
            if (!VD)
                return;

            // Check if it's a local variable (not a parameter)
            if (!VD->isThisDeclarationADefinition() || VD->isParmVarDecl())
                return;

            // Check if the type is std::string
            clang::QualType VDType = VD->getType();
            const clang::Type *CanType = VDType.getCanonicalType().getTypePtr();
            if (!CanType || !CanType->getAs<clang::RecordType>()) {
                return;
            }
            const clang::RecordType *RT = CanType->getAs<clang::RecordType>();
            const clang::CXXRecordDecl *RD = dyn_cast<clang::CXXRecordDecl>(RT->getDecl());
            if (!RD || !RD->getQualifiedNameAsString().startswith("std::basic_string")) {
                return;
            }

            // Now, proceed to replace the return statement
            std::string varName = DRE->getNameInfo().getIdentifier().str();
            std::string newReturn = "return std::string_view(" + varName + ".substr(0,3));";

            // Replace the original return statement
            SourceLocation startLoc = RS->getBeginLoc();
            SourceLocation endLoc = RS->getEndLoc();
            CharSourceRange range = CharSourceRange::getTokenRange(SourceRange(startLoc, endLoc));
            Rewrite.ReplaceText(range, newReturn);
        }
    }
}
  
void MutatorFrontendAction_430::MutatorASTConsumer_430::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher matcher = returnStmt(
        hasAncestor(cxxFunctionDecl(
            hasReturnType(qualType(
                hasCanonicalType(
                    recordType(
                        hasDeclaration(cxxRecordDecl(
                            hasName("std::basic_string")))) ))))),
        hasReturnValue(declRefExpr(to(varDecl(
            hasType(qualType(
                hasCanonicalType(
                    recordType(
                        hasDeclaration(cxxRecordDecl(
                            hasName("std::basic_string")))) ))),
            unless(isParmVarDecl())
        ))))
    ).bind("returnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);

    // Add include for string_view
    SourceManager &SM = Context.getSourceManager();
    FileID FID = SM.getMainFileID();
    TheRewriter.InsertText(SM.getLocForStartOfFile(FID), "#include <string_view>\n", true, true);
}