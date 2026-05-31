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