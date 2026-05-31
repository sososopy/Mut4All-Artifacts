//header file
#pragma once
#include "Mutator_base.h"

/**
 * incomplete_struct_initialization_5
 */ 
class MutatorFrontendAction_5 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(5)

private:
    class MutatorASTConsumer_5 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_5(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::RecordDecl*, std::vector<const clang::FieldDecl*>> structFields;
    };
};

//source file
#include "../include/incomplete_struct_initialization_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("StructDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        
        if (RD->isCompleteDefinition() && RD->isStruct()) {
            std::vector<const clang::FieldDecl*> fields;
            for (auto *field : RD->fields()) {
                fields.push_back(field);
            }
            structFields[RD] = fields;
        }
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (VD->hasInit() && VD->getType()->isStructureType()) {
            const clang::RecordDecl *recordDecl = VD->getType()->getAsStructureType()->getDecl();
            if (structFields.find(recordDecl) != structFields.end()) {
                auto &fields = structFields[recordDecl];
                if (fields.size() > 1) {
                    // Omit one of the fields to make the struct incomplete
                    std::string structText = "struct " + recordDecl->getNameAsString() + " {";
                    for (size_t i = 0; i < fields.size() - 1; ++i) {
                        structText += fields[i]->getType().getAsString() + " " + fields[i]->getNameAsString() + "; ";
                    }
                    structText += "};\n";
                    Rewrite.ReplaceText(recordDecl->getSourceRange(), structText);
                }
            }
        }
    }
}

void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher structMatcher = recordDecl(isStruct()).bind("StructDecl");
    DeclarationMatcher varMatcher = varDecl(hasInitializer(anything())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.matchAST(Context);
}