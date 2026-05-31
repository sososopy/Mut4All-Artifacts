//header file
#pragma once
#include "Mutator_base.h"

/**
 * ObjectiveCXXStructWithNoUniqueEmptyMember_480
 */ 
class MutatorFrontendAction_480 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(480)

private:
    class MutatorASTConsumer_480 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_480(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ObjectiveCXXStructWithNoUniqueEmptyMember_480.h"

// ========================================================================================================
#define MUT480_OUTPUT 1

void MutatorFrontendAction_480::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *structDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("structDecl")) {
        // Filter nodes in header files
        if (!Result.Context->getSourceManager().isWrittenInMainFile(structDecl->getLocation()))
            return;

        // Check if the struct already has a [[no_unique_address]] member of an empty struct
        bool hasEmptyMember = false;
        for (const auto *field : structDecl->fields()) {
            if (field->hasAttr<clang::NoUniqueAddressAttr>()) {
                QualType fieldType = field->getType();
                if (const auto *recordType = fieldType.getTypePtr()->getAsCXXRecordDecl()) {
                    if (recordType->getNameAsString() == "Empty" && recordType->getNumFields() == 0) {
                        hasEmptyMember = true;
                        break;
                    }
                }
            }
        }
        if (hasEmptyMember)
            return;

        // Check if the empty struct exists in the AST
        bool emptyStructExists = false;
        ASTContext *context = Result.Context;
        for (const auto *decl : context->getTranslationUnitDecl()->decls()) {
            if (const auto *record = dyn_cast<clang::CXXRecordDecl>(decl)) {
                if (record->getNameAsString() == "Empty" && record->getNumFields() == 0) {
                    emptyStructExists = true;
                    break;
                }
            }
        }

        // If not, insert the empty struct at the beginning of the file
        if (!emptyStructExists) {
            SourceManager &SM = Rewrite.getSourceMgr();
            FileID mainFileID = SM.getMainFileID();
            unsigned mainFileStart = SM.getFileOffset(mainFileID);
            Rewrite.InsertText(mainFileStart, "struct Empty {};\n", true, true);
        }

        // Insert the [[no_unique_address]] member into the struct
        SourceLocation openBraceLoc = structDecl->getBraceRange().getBegin();
        Rewrite.InsertTextAfter(openBraceLoc, "    [[no_unique_address]] Empty padding;\n");
    }
}
  
void MutatorFrontendAction_480::MutatorASTConsumer_480::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = objcInterfaceDecl(hasAnyDirectIvar(cxxRecordDecl().bind("structDecl"))).bind("objcInterface");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}