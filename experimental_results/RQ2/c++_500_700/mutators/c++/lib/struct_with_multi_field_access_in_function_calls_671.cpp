//source file
#include "../include/struct_with_multi_field_access_in_function_calls_671.h"

// ========================================================================================================
#define MUT671_OUTPUT 1

void MutatorFrontendAction_671::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("structDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        
        if (!RD->isStruct())
            return;

        int fieldCount = 0;
        bool hasMultipleTypes = false;
        std::set<std::string> fieldTypes;

        for (const auto *field : RD->fields()) {
            fieldCount++;
            fieldTypes.insert(field->getType().getAsString());
            if (fieldTypes.size() > 1) {
                hasMultipleTypes = true;
                break;
            }
        }

        if (fieldCount >= 2 && hasMultipleTypes) {
            structDecls.push_back(RD);
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        for (const auto *RD : structDecls) {
            std::string structName = RD->getNameAsString();
            std::string callText = "__builtin_dump_struct(&obj, existingFunction);";

            for (auto *param : FD->parameters()) {
                if (param->getType().getAsString().find(structName) != std::string::npos) {
                    SourceLocation insertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);
                    Rewrite.InsertText(insertLoc, "\n" + callText);
                    break;
                }
            }
        }
    }
}

void MutatorFrontendAction_671::MutatorASTConsumer_671::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher structMatcher = recordDecl(isStruct()).bind("structDecl");
    DeclarationMatcher functionMatcher = functionDecl(hasBody(stmt())).bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}